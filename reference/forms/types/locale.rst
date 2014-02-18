.. index::
   single: Forms; Fields; locale

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2014/02/16

locale フィールドタイプ
=======================

``locale`` タイプは、大きなロケール（言語+国）のリストから選ぶことが出来る ``ChoiceType`` のサブセットです。加えて、ロケール名がユーザーの言語の中に表示されます。

各ロケールの値は2文字の `ISO 639-1`_ *言語* コード（例 ``fr`` ）か、または、その言語コードにアンダースコアー （ ``_`` ） で `ISO 3166-1 alpha-2`_ *国* コードを続けたもの（例 French/France は ``fr_FR``）です。

.. note::

   ユーザーのロケールは :phpmethod:`Locale::getDefault` を使用して推測されています。

``choice`` タイプとは異なり、\ ``choices`` または ``choice_list`` オプションを特定する必要はなく、フィールドタイプとして自動的に大きなロケールのリストを使います。それらのオプションのどちらかを手動で設定することは *出来ます* が、その場合は ``choice`` タイプを直接使うべきです。

+-------------+------------------------------------------------------------------------+
| 対応するタグ| 様々なタグとして利用可能 (see :ref:`forms-reference-choice-tags`)      |
+-------------+------------------------------------------------------------------------+
| 上書きされた| - `choices`_                                                           |
| オプション  |                                                                        |
+-------------+------------------------------------------------------------------------+
| 継承された  | - `multiple`_                                                          |
| オプション  | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `empty_value`_                                                       |
|             | - `error_bubbling`_                                                    |
|             | - `error_mapping`_                                                     |
|             | - `empty_data`_                                                        |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `label_attr`_                                                        |
|             | - `data`_                                                              |
|             | - `read_only`_                                                         |
|             | - `disabled`_                                                          |
|             | - `mapped`_                                                            |
+-------------+------------------------------------------------------------------------+
| 親タイプ    | :doc:`choice </reference/forms/types/choice>`                          |
+-------------+------------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LocaleType`   |
+-------------+------------------------------------------------------------------------+

上書きされたオプション
----------------------

choices
~~~~~~~

**デフォルト**: ``Symfony\Component\Intl\Intl::getLocaleBundle()->getLocaleNames()``

``choices`` オプションはデフォルトですべてのロケールを設定します。これは言語を特定するデフォルトのロケールとして使われます。

継承されたオプション
--------------------

以下のオプションは :doc:`choice </reference/forms/types/choice>` タイプを継承しています:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. _`ISO 639-1`: http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
.. _`ISO 3166-1 alpha-2`: http://en.wikipedia.org/wiki/ISO_3166-1#Current_codes

.. 2014/02/16 yositani2002 d9c6a853a84754e8fc8379bf61b70c02493e305f
