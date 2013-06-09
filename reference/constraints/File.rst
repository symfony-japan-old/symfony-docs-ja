File
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が有効な "ファイル" であることを検証します。ファイルとは次のどれかになります。

* 存在するファイルへのパスを表す文字列、または ``__toString()`` メソッドを持つオブジェクト

* 有効な :class:`Symfony\\Component\\HttpFoundation\\File\\File` クラス、およびサブクラスのオブジェクト。これには :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile` クラスのオブジェクトも含まれます。

この制約は、フォームの :doc:`file</reference/forms/types/file>` フォームタイプと合わせて使われます。

.. tip::

    検証対象のファイルが画像である場合は :doc:`Image</reference/constraints/Image>` 制約を使ってください。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `maxSize`_                                                        |
|                | - `mimeTypes`_                                                      |
|                | - `maxSizeMessage`_                                                 |
|                | - `mimeTypesMessage`_                                               |
|                | - `notFoundMessage`_                                                |
|                | - `notReadableMessage`_                                             |
|                | - `uploadIniSizeErrorMessage`_                                      |
|                | - `uploadFormSizeErrorMessage`_                                     |
|                | - `uploadErrorMessage`_                                             |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\File`           |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`  |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

この制約は、フォームの :doc:`file</reference/forms/types/file>` フォームタイプでレンダリングするプロパティに対して使います。たとえば、著者情報のフォームを作成しており、著者情報の PDF ファイルをアップロードできるようにするとします。フォームでは、\ ``bioFile`` プロパティを ``file`` タイプにします。\ ``Author`` クラスは次のようになります。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $bioFile;

        public function setBioFile(File $file = null)
        {
            $this->bioFile = $file;
        }

        public function getBioFile()
        {
            return $this->bioFile;
        }
    }

``bioFile`` フィールドの ``File`` オブジェクトが有効で、特定のファイルサイズ以下の有効な PDF ファイルであることを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                bioFile:
                    - File:
                        maxSize: 1024k
                        mimeTypes: [application/pdf, application/x-pdf]
                        mimeTypesMessage: Please upload a valid PDF
                        

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\File(
             *     maxSize = "1024k",
             *     mimeTypes = {"application/pdf", "application/x-pdf"},
             *     mimeTypesMessage = "Please upload a valid PDF"
             * )
             */
            protected $bioFile;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="bioFile">
                <constraint name="File">
                    <option name="maxSize">1024k</option>
                    <option name="mimeTypes">
                        <value>application/pdf</value>
                        <value>application/x-pdf</value>
                    </option>
                    <option name="mimeTypesMessage">Please upload a valid PDF</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bioFile', new Assert\File(array(
                    'maxSize' => '1024k',
                    'mimeTypes' => array(
                        'application/pdf',
                        'application/x-pdf',
                    ),
                    'mimeTypesMessage' => 'Please upload a valid PDF',
                )));
            }
        }

``bioFile`` プロパティが検証され、実際に存在するファイルであることが保証されます。ファイルサイズと MIME タイプオプションも指定しているので、同時に検証が行われます。

オプション
----------

maxSize
~~~~~~~

**型**: ``mixed``

検証対象のファイルのサイズが指定した数値以下であることを検証します。次のような形式でファイルサイズを指定します。

* **バイト**: ``maxSize`` をバイト数単位で指定するには、単純に数値のみの値を指定します (``4096``)。

* **キロバイト**: ``maxSize`` をキロバイト単位で指定するには、数値と小文字の "k" で指定します (``200k``)。

* **メガバイト**: ``maxSize`` をメガバイト単位で指定するには、数値と大文字の "M" で指定します (``4M``)。

mimeTypes
~~~~~~~~~

**型**: ``array`` または ``string``

検証対象のファイルの MIME タイプを検証します。文字列で指定した場合は、MIME タイプが一致することを、配列で指定した場合は、MIME タイプがコレクション中に存在することが検証されます。

既存の MIME タイプの一覧については、\ `IANA Web サイト`_\ を参照してください。

maxSizeMessage
~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file is too large ({{ size }}). Allowed maximum size is {{ limit }}``

検証対象のファイルサイズが `maxSize`_ オプションで指定したサイズよりも大きかった場合に、このメッセージが表示されます。

mimeTypesMessage
~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルトt**: ``The mime type of the file is invalid ({{ type }}). Allowed mime types are {{ types }}``

検証対象のファイルの MIME タイプが `mimeTypes`_ オプションに対して有効な値ではない場合に、このメッセージが表示されます。

notFoundMessage
~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file could not be found``

入力されたパスにファイルが見つからない場合に、このメッセージが表示されます。このエラーは、文字列のパスを検証対象としている場合にのみ発生します。\ ``File`` オブジェクトを使っている場合は、無効なファイルパスでオブジェクトを作成することができません。

notReadableMessage
~~~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file is not readable``

入力されたパスにファイルが存在するが PHP の ``is_readable`` 関数の検査が失敗する場合に、このメッセージが表示されます。

uploadIniSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file is too large. Allowed maximum size is {{ limit }}``

アップロードされたファイルのサイズが PHP.ini の ``upload_max_filesize`` 設定値よりも大きい場合に、このメッセージが表示されます。

uploadFormSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file is too large``

アップロードされたファイルのサイズが HTML の file INPUT タグに指定された値よりも大きい場合に、このメッセージが表示されます。

uploadErrorMessage
~~~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The file could not be uploaded``

ファイルアップロードに失敗したり、ディスクへの書き込みができなかったなど、何らかの理由でファイルがアップロードできなかった場合に、このメッセージが表示されます。


.. _`IANA Web サイト`: http://www.iana.org/assignments/media-types/index.html

.. 2013/06/09 hidenorigoto 5f4e6ef7fa26bd9aa160a3f01e3e1f25505de1c0
