Image
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

Image 制約は :doc:`File</reference/constraints/File>` 制約とほぼ同じように機能しますが、画像ファイル向けに `mimeTypes`_ オプションと `mimeTypesMessage` オプションが自動的に設定されます。

また、Symfony 2.1 以降では画像の幅と高さを検証するオプションが追加されました。

この制約の基本機能については、\ :doc:`File</reference/constraints/File>` 制約のドキュメントを参照してください。

+----------------+----------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`          |
+----------------+----------------------------------------------------------------------+
| オプション     | - `mimeTypes`_                                                       |
|                | - `minWidth`_                                                        |
|                | - `maxWidth`_                                                        |
|                | - `maxHeight`_                                                       |
|                | - `minHeight`_                                                       |
|                | - `mimeTypesMessage`_                                                |
|                | - `sizeNotDetectedMessage`_                                          |
|                | - `maxWidthMessage`_                                                 |
|                | - `minWidthMessage`_                                                 |
|                | - `maxHeightMessage`_                                                |
|                | - `minHeightMessage`_                                                |
|                | - See :doc:`File</reference/constraints/File>` for inherited options |
+----------------+----------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\File`            |
+----------------+----------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`   |
+----------------+----------------------------------------------------------------------+

基本的な使い方
--------------

この製薬は、フォームの :doc:`file</reference/forms/types/file>` フォームタイプでレンダリングするプロパティに対して使います。たとえば、著者情報のフォームを作成しており、著者の顔写真の画像ファイルをアップロードできるようにするとします。フォームでは、\ ``headshot`` プロパティを ``file`` タイプにします。\ ``Author`` クラスは次のようになります。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $headshot;

        public function setHeadshot(File $file = null)
        {
            $this->headshot = $file;
        }

        public function getHeadshot()
        {
            return $this->headshot;
        }
    }

``headshot`` フィールドの ``File`` オブジェクトが有効な画像で、特定のサイズ以下であることを保証するには、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            properties:
                headshot:
                    - Image:
                        minWidth: 200
                        maxWidth: 400
                        minHeight: 200
                        maxHeight: 400
                        

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Image(
             *     minWidth = 200,
             *     maxWidth = 400,
             *     minHeight = 200,
             *     maxHeight = 400
             * )
             */
            protected $headshot;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="headshot">
                <constraint name="Image">
                    <option name="minWidth">200</option>
                    <option name="maxWidth">400</option>
                    <option name="minHeight">200</option>
                    <option name="maxHeight">400</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        // ...

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Image;

        class Author
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('headshot', new Image(array(
                    'minWidth' => 200,
                    'maxWidth' => 400,
                    'minHeight' => 200,
                    'maxHeight' => 400,
                )));
            }
        }

``headshot`` プロパティが検証され、実際に存在するファイルであることが保証されます。画像の幅と高さが特定の大きさ以内であることも検証されます。

オプション
----------

この制約では、\ :doc:`File</reference/constraints/File>` 制約のすべてのオプションも利用可能です。ただし、2 つのオプションはデフォルトで設定され、他のいくつかも動作が変更されています。

mimeTypes
~~~~~~~~~

**型**: ``array`` または ``string`` **デフォルト**: ``image/*``

既存の画像に関する MIME タイプの一覧については、\ `IANA Web サイト`_\ を参照してください。

mimeTypesMessage
~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``This file is not a valid image``

minWidth
~~~~~~~~

**型**: ``integer``

画像の幅が指定した数値のピクセル数以上であることが検証されます。

maxWidth
~~~~~~~~

**型**: ``integer``

画像の幅が指定した数値のピクセル数以下であることが検証されます。

minHeight
~~~~~~~~~

**型**: ``integer``

画像の高さが指定した数値のピクセル数以上であることが検証されます。

maxHeight
~~~~~~~~~

**型**: ``integer``

画像の高さが指定した数値のピクセル数以下であることが検証されます。

sizeNotDetectedMessage
~~~~~~~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The size of the image could not be detected``

システムで画像のサイズを検出できなかった場合に、このメッセージが表示されます。画像サイズに関する 4 つの制約の 1 つでも設定されていると、このエラーが発生する可能性があります。

maxWidthMessage
~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The image width is too big ({{ width }}px). Allowed maximum width is {{ max_width }}px``

画像の幅が `maxWidth`_ オプションで指定した幅よりも大きかった場合に、このメッセージが表示されます。

minWidthMessage
~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The image width is too small ({{ width }}px). Minimum width expected is {{ min_width }}px``

画像の幅が `minWidth`_ オプションで指定した幅よりも小さかった場合に、このメッセージが表示されます。

maxHeightMessage
~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The image height is too big ({{ height }}px). Allowed maximum height is {{ max_height }}px``

画像の高さが `maxHeight`_ オプションで指定した高さよりも大きかった場合に、このメッセージが表示されます。

minHeightMessage
~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``The image height is too small ({{ height }}px). Minimum height expected is {{ min_height }}px``

画像の高さが `minHeight`_ オプションで指定した高さよりも小さかった場合に、このメッセージが表示されます。

.. _`IANA Web サイト`: http://www.iana.org/assignments/media-types/image/index.html

.. 2013/06/09 hidenorigoto 4346f75f05a5ee010d0148ea251e99c7f6a02c38
