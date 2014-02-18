.. index::
   single: Forms; Fields; country

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2014/02/16


country フィールドタイプ
========================

``country`` タイプは世界の国を表示する ``ChoiceType`` のサブセットです。加えて、利用者の言語の中に国名が表示されます。

各国の値は2文字の国コードです。

.. note::

   ユーザーのロケールは :phpmethod:`Locale::getDefault` を使用して推測されています。

``choice`` タイプとは異なり、\ ``choices`` または ``choice_list`` オプションを特定する必要はなく、
フィールドタイプとして自動的に世界のすべての国を使います。それらのオプションのどちらかを手動で設定する
ことは *出来ます* が、その場合は ``choice`` タイプを直接使うべきです。

+-------------+-----------------------------------------------------------------------+
| 対応するタグ| 様々なタグとして利用可能 (see :ref:`forms-reference-choice-tags`)     |
+-------------+-----------------------------------------------------------------------+
| 上書きされた| - `choices`_                                                          |
| オプション  |                                                                       |
+-------------+-----------------------------------------------------------------------+
| 継承された  | - `multiple`_                                                         |
| オプション  | - `expanded`_                                                         |
|             | - `preferred_choices`_                                                |
|             | - `empty_value`_                                                      |
|             | - `error_bubbling`_                                                   |
|             | - `error_mapping`_                                                    |
|             | - `empty_data`_                                                       |
|             | - `required`_                                                         |
|             | - `label`_                                                            |
|             | - `label_attr`_                                                       |
|             | - `data`_                                                             |
|             | - `read_only`_                                                        |
|             | - `disabled`_                                                         |
|             | - `mapped`_                                                           |
+-------------+-----------------------------------------------------------------------+
| 親タイプ    | :doc:`choice </reference/forms/types/choice>`                         |
+-------------+-----------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CountryType` |
+-------------+-----------------------------------------------------------------------+

上書きされたオプション
----------------------

choices
~~~~~~~

**デフォルト**: ``Symfony\Component\Intl\Intl::getRegionBundle()->getCountryNames()``

``country`` タイプは ``choices`` オプションにデフォルトですべての国のリストを設定します。
国名の翻訳にはロケールが利用されます。

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

.. 2014/02/16 yositani2002 d9c6a853a84754e8fc8379bf61b70c02493e305f
